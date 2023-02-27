declare module 'crypt3-bindings' {
    function encrypt(passwd: string, salt?: string): string;

    export {
        encrypt,
    }
}